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
//<ID> 1760
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *threshold = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(meta, "threshold", threshold);

    // step 2: Configure
    double set_result = cJSON_SetNumberHelper(threshold, 99.5);
    (void)set_result;
    cJSON *insert_num = cJSON_CreateNumber(15.5);
    cJSON_InsertItemInArray(values, 1, insert_num);

    // step 3: Operate
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemViaPointer(values, n2, replacement);
    char *printed = cJSON_PrintUnformatted(root);
    double thr_val = cJSON_GetNumberValue(threshold);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + ((int)thr_val % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}