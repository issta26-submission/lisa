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
//<ID> 519
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(values, false_item);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *status = cJSON_AddTrueToObject(root, "valid");

    // step 2: Configure
    int size_before = cJSON_GetArraySize(values);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    double computed = first_val + (double)size_before;
    cJSON *meta = cJSON_CreateObject();
    cJSON *comp_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(meta, "computed", comp_num);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON_DeleteItemFromArray(values, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double computed_extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "meta"), "computed"));
    double first_after = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    (void)status;
    (void)computed_extracted;
    (void)first_after;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}