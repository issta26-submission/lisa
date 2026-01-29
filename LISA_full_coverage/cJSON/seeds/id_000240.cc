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
//<ID> 240
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
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON *str = cJSON_CreateString("example");
    cJSON_AddItemToObject(child, "num", num);
    cJSON_AddItemToObject(child, "str", str);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON_bool had_child_before = cJSON_HasObjectItem(root, "child");
    cJSON *detached = cJSON_DetachItemViaPointer(root, child);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON_bool has_child_after = cJSON_HasObjectItem(root, "child");

    // step 3: Operate and Validate
    cJSON *dup = cJSON_Duplicate(detached, 1);
    cJSON_bool equal = cJSON_Compare(detached, dup, 1);
    cJSON *num_node = cJSON_GetObjectItem(detached, "num");
    double val = cJSON_GetNumberValue(num_node);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    int summary = (int)had_child_before + (int)has_child_after + (int)equal + (int)(printed != NULL) + (int)val;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}