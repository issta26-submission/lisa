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
//<ID> 1070
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"number\":10,\"replace_me\":0,\"values\":[1,2,3]}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    double dbls[] = {3.14, 2.718, 1.618};
    cJSON *dbl_array = cJSON_CreateDoubleArray(dbls, 3);
    cJSON_AddItemToObject(root, "dbls", dbl_array);
    cJSON *newnum = cJSON_CreateNumber(123.456);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_me", newnum);
    cJSON_AddTrueToObject(root, "active");

    // step 3: Operate
    cJSON *values = cJSON_GetObjectItem(root, "values");
    int values_size = cJSON_GetArraySize(values);
    cJSON *first_val = cJSON_GetArrayItem(values, 0);
    double first_num = cJSON_GetNumberValue(first_val);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);

    // step 4: Validate & Cleanup
    (void)values_size;
    (void)first_num;
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}