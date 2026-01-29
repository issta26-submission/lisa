#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("value");
    cJSON_AddItemToObject(child, "answer", num);
    cJSON_AddItemToObject(child, "key", str);

    // step 2: Configure
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON *detached = cJSON_DetachItemViaPointer(root, child);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON *other = cJSON_Duplicate(detached, 1);

    // step 3: Operate and Validate
    cJSON_bool equal_before = cJSON_Compare(detached, other, 1);
    cJSON *ans_node = cJSON_GetObjectItem(detached, "answer");
    double original_val = cJSON_GetNumberValue(ans_node);
    double new_val = cJSON_SetNumberHelper(ans_node, original_val * 2.0);
    cJSON_bool has_key = cJSON_HasObjectItem(detached, "key");
    cJSON *key_node = cJSON_GetObjectItem(detached, "key");
    cJSON *detached_key = cJSON_DetachItemViaPointer(detached, key_node);
    cJSON_AddItemToObject(root, "rekey", detached_key);
    cJSON_bool equal_after = cJSON_Compare(detached, other, 0);
    char *printed_unformatted = cJSON_PrintUnformatted(root);
    char *printed_other = cJSON_Print(other);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = (int)new_val + (int)has_child + (int)has_key + (int)equal_before + (int)equal_after + cJSON_GetArraySize(root);
    cJSON_AddNumberToObject(root, "summary", (double)summary);

    // step 4: Cleanup
    cJSON_free(printed_unformatted);
    cJSON_free(printed_other);
    cJSON_free(tmp);
    cJSON_Delete(other);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}