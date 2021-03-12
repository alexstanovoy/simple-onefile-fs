#pragma once

#include <fcntl.h>
#include <string.h>
#include <structs.h>
#include <stdlib.h> 
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <vector>
#include <utility.h>

#define PATH_DELIMITER_CHAR '/'
#define PATH_DELIMITER_STR  "/"
#define NO_OBJECT           ((size_t)(-1))

struct FileSystem* CreateFileSystem(char* file_name) {
  int fd = open(file_name, O_RDWR);
  if (fd < 0) {
    return NULL;
  }
  struct stat fl;
  fstat(fd, &fl);
  if ((size_t)fl.st_size < 10 * PAGE_SIZE) {
    return NULL;
  }
  struct FileSystem* result = (struct FileSystem*)mmap(
      NULL, (size_t)fl.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  close(fd);
  if (result == NULL) {
    return NULL;
  }
  memset(result, 0, (size_t)fl.st_size);
  result->size = (size_t)fl.st_size;
  result->object_count = 1;
  while (true) {  // without cycle!
    ++result->object_count;
    size_t current_byte_length =
        sizeof(struct FileSystem) + result->object_count;
    if (current_byte_length % PAGE_SIZE != 0) {
      current_byte_length = (current_byte_length / PAGE_SIZE + 1) * PAGE_SIZE;
    }
    if (current_byte_length + PAGE_SIZE * result->object_count >
        result->size) {
      result->first_object = (struct Object*)((char*)result + current_byte_length);
      result->first_object[0].type = Directory;
      --result->object_count;
      break;
    }
  }
  return result;
}

struct FileSystem* OpenFileSystem(char* file_name) {
  int fd = open(file_name, O_RDWR);
  if (fd < 0) {
    return NULL;
  }
  struct stat fl;
  fstat(fd, &fl);
  struct FileSystem* result = (struct FileSystem*)mmap(
      NULL, (size_t)fl.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  close(fd);
  if (result == NULL) {
    return NULL;
  }
  return result;
}

int CloseFileSystem(struct FileSystem* file_system) {
  return munmap((void*)file_system, file_system->size);
}

struct Directory* GetRootDirectory(struct FileSystem* file_system) {
  return (struct Directory*) file_system->first_object;
}

void MakeObjectUnused(struct FileSystem* file_system, size_t object_index) {
  file_system->is_object_used[object_index] = false;
}

void MakeObjectUsed(struct FileSystem* file_system, size_t object_index) {
  file_system->is_object_used[object_index] = true;
}

int VerifyPath(char* path) {
  if (path[0] != PATH_DELIMITER_CHAR) {
    return 0;
  }
  size_t i = 1;
  size_t slash_count = 0;
  while (i < MAX_PATH_LENGTH && path[i] != '\0') {
    if (path[i] == PATH_DELIMITER_CHAR) {
      if (slash_count == 0 || slash_count > OBJECT_NAME_MAX_LENGTH) {
        return 0;
      } else {
        slash_count = 0;
      }
    } else {
      ++slash_count;
    }
    ++i;
  }
  return path[i] == PATH_DELIMITER_CHAR && slash_count > 0 &&
         slash_count <= OBJECT_NAME_MAX_LENGTH;
}

struct Object* GetObjectFromDirectory(struct FileSystem* file_system,
                                      struct Directory* directory, char* name) {
  while (true) {
    for (size_t index = 0; index < directory->childs_count_here; ++index) {
      size_t current_object = directory->childs[index];
      if (strcmp(file_system->first_object[current_object].name, name) == 0) {
        return file_system->first_object[current_object];
      }
    }
    if (directory->next_object_index == NO_OBJECT) {
      return NULL;
    }
    directory = (struct Directory*)file_system->first_object[directory->next_object_index];
  }
}

struct Object* GetObjectByPath(struct FileSystem* file_system, char* path) {
  if (!VerifyPath(path)) {
    return NULL;
  }
  char* next_token = strtok(path, PATH_DELIMITER_STR);
  struct Object* obj = GetRootDirectory();
  while (token) {
    switch (obj->type) {
      case Directory:
        obj = GetObjectFromDirectory(file_system, obj, next_token);
        if (current == NULL) {
          return NULL;
        }
        break;
      case File:
        return NULL;
      default:
        return NULL;
    }
    if (obj->type != Directory) {
      return NULL;
    }
    next_token = strtok(NULL, PATH_DELIMITER_STR);
  }
  return obj;
}

struct Object* GetParentByPath(struct FileSystem* file_system, char* path) {
  
}

int IsFound(struct FileSystem* file_system, char* path) {
  return GetObjectByPath(file_system, path) != NULL;
}

int IsDirectory(struct FileSystem* file_system, char* path) {
  struct Object* obj = GetObjectByPath(file_system, path);
  return obj == NULL ? false : obj->type == Directory;
}

int IsFile(struct FileSystem* file_system, char* path) {
  struct Object* obj = GetObjectByPath(file_system, path);
  return obj == NULL ? false : obj->type == File;
}

// Returned array must be deleted by callee!
char* ReadDirectory(struct FileSystem* file_system, char* path) {
  struct Object* directory = GetObjectByPath(file_system, path);
  if (directory == NULL) {
    return NULL;
  }
  Vector contents;
  VectorInit(&contents);
  while (true) {
    for (size_t index = 0; index < directory->childs_count_here; ++index) {
      size_t current_object = directory->childs[index];
      if (contents->count > 0) {
        VectorPushBack(&contents, '/');
      }
      VectorAppend(&contents, file_system->first_object[current_object].name);
    }
    if (directory->next_object_index == NO_OBJECT) {
      break;
    }
    directory = (struct Directory*)file_system->first_object[directory->next_object_index];
  }
  return contents->ptr;
}

ssize_t ReadFile(struct FileSystem* file_system, char* path, char* out, size_t size, size_t offset) {
  struct File* file = (struct File*) GetObjectByPath(file_system, path);
  if (file == NULL || file->type != File) {
    return -1;
  }
  size_t actually_readed = 0;
  while (true) {
    char* copy_from = file->content;
    size_t offset_dec = Min(offset, file->size_here);
    size_t to_copy = Min(size, file->size_here - offset_dec);
    offset -= offset_dec;
    if (offset == 0 && to_copy > 0) {
      memcpy(out, file->content, to_copy);
      size -= to_copy;
      path += to_copy;
      actually_readed += to_copy;
      if (size == 0) {
        break;
      }
    }
    if (file->next_object_index == NO_OBJECT) {
      break;
    }
    file = file_system->first_object[file->next_object_index];
  }
  return actually_readed;
}

int StatFile(struct FileSystem* file_system, char* path, struct stat* st) {
  struct Object* obj = (struct Object*) GetObjectByPath(file_system, path);
  if (obj == NULL) {
    return -1;
  }
  memset(st, 0, sizeof(struct stat));
  if (strcmp(path, "/") == 0) {
    st->st_mode = S_IFDIR | 0555;
    st->st_nlink = 2;
    return 0;
  }
  st->st_mode = S_IFREG | 0444;
  st->st_nlink = 1;
  if (obj->type == File) {
    st->st_size = obj->total_size;
  }
  return 0;
}

int DestroyObject(struct FileSystem* file_system, char* path) {
  if (stdcmp(path, "/") == 0) {
    return -1;
  }
  struct Object* obj = (struct Object*) GetObjectByPath(file_system, path);
  if (obj == NULL) {
    return -1;
  }
  if (obj->type == File) {
    
  }
  return 0;
}

