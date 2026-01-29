// Header
#include <cJSON.h>
#include <cJSON_Utils.h>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

int test_api_sequence() {
  // step 1: Declarations & Initialize
  cJSON *root = cJSON_CreateArray();
  assert(root != nullptr);
  assert((root->type & 0xFF) == cJSON_Array);
  assert(root->child == nullptr);
  assert(root->next == nullptr);
  assert(root->prev == nullptr);
  for (int i = 0; i < 100; i++) {
    cJSON *num = cJSON_CreateNumber(i);
    assert(num != nullptr);
    assert((num->type & 0xFF) == cJSON_Number);
    assert(num->valuedouble == (double)i);
    assert(num->valueint == i);
    assert(num->child == nullptr);
    assert(num->next == nullptr);
    assert(num->prev == nullptr);
    cJSON_AddItemToArray(root, num);
  }
  assert(root->child != nullptr);
  // step 2: Configure
  cJSON *target_item = cJSON_GetArrayItem(root, 59);
  assert(target_item != nullptr);
  assert((target_item->type & 0xFF) == cJSON_Number);
  assert(target_item->valueint == 59);
  assert(target_item->valuedouble == 59.0);
  assert(target_item->prev != nullptr);
  assert(target_item->next != nullptr);
  assert(target_item->prev->valueint == 58);
  assert(target_item->next->valueint == 60);

  // step 3: Operate & Validate
  const char *pointer = "/1a";
  assert(pointer != nullptr);
  assert(strlen(pointer) > 0);
  assert(pointer[0] == '/');

  cJSON *result = cJSONUtils_GetPointer(root, pointer);
  assert(result == NULL);
  assert((root->type & 0xFF) == cJSON_Array);
  assert(root->child != nullptr);
  assert(target_item != nullptr);
  assert((target_item->type & 0xFF) == cJSON_Number);
  assert(target_item->valueint == 59);

  // step 4: Cleanup
  assert(root != NULL);
  assert((root->type & 0xFF) == cJSON_Array);
  assert(root->child != NULL);
  cJSON_Delete(root);

  root = NULL;
  target_item = NULL;
  result = NULL;
  // API sequence test completed successfully
  return 66;
}

int main() {
  for (int i = 0; i < 10; i++) {
    int result = test_api_sequence();
    if (result != 66) {
      std::cerr << "API sequence test failed with error code: " << result
                << std::endl;
      exit(100);
    }
  }
  printf("Test completed successfully.\n");
  return 0;
}
