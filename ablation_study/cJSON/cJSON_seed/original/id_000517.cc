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
//<ID> 517
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(20.25);
    cJSON *n3 = cJSON_CreateNumber(-5.75);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToArray(items, n3);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "toggled");
    int initial_count = cJSON_GetArraySize(items);
    cJSON_DeleteItemFromArray(items, 1);
    int post_delete_count = cJSON_GetArraySize(items);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(meta, "first", first_val);

    // step 3: Operate
    double computed = first_val + (double)post_delete_count + (double)initial_count;
    cJSON_AddNumberToObject(meta, "computed", computed);
    size_t buf_len = 64 + (size_t)post_delete_count * 8;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    char *snapshot = cJSON_PrintUnformatted(root);
    double extracted_before = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "first"));

    // step 4: Validate & Cleanup
    double extracted_computed = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "computed"));
    (void)extracted_before;
    (void)extracted_computed;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}