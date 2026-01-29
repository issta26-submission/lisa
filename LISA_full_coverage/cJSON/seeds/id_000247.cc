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
//<ID> 247
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *childA = cJSON_CreateObject();
    cJSON *childB = cJSON_CreateObject();
    cJSON *numA = cJSON_CreateNumber(42.0);
    cJSON *strA = cJSON_CreateString("value");
    cJSON *numB = cJSON_CreateNumber(42.0);
    cJSON *strB = cJSON_CreateString("value");
    cJSON_AddItemToObject(childA, "num", numA);
    cJSON_AddItemToObject(childA, "text", strA);
    cJSON_AddItemToObject(childB, "num", numB);
    cJSON_AddItemToObject(childB, "text", strB);
    cJSON_AddItemToObject(root, "childA", childA);
    cJSON_AddItemToObject(root, "childB", childB);

    // step 2: Configure
    cJSON_bool hasA = cJSON_HasObjectItem(root, "childA");
    cJSON *detached = cJSON_DetachItemViaPointer(root, childA);
    cJSON_bool equal = cJSON_Compare(detached, childB, 1);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *num_from_childB = cJSON_GetObjectItem(childB, "num");
    double val = cJSON_GetNumberValue(num_from_childB);
    int present_flag = (int)hasA;
    int eq_flag = (int)equal;
    int summary = (int)val + present_flag + eq_flag + cJSON_GetArraySize(childB);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}