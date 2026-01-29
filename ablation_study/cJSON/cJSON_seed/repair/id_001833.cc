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
//<ID> 1833
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"name\" : \"alpha\", \"payload\" : { \"count\" : 3 }, \"array\" : [ 1, 2, 3 ] } ";
    size_t raw_len = strlen(raw_json);
    char *json_buffer = (char *)cJSON_malloc(raw_len + 1);
    memcpy(json_buffer, raw_json, raw_len + 1);
    cJSON_Minify(json_buffer);
    cJSON *root = cJSON_Parse(json_buffer);

    // step 2: Configure
    cJSON *added_obj = cJSON_AddObjectToObject(root, "added_obj");
    cJSON *payload = cJSON_GetObjectItem(root, "payload");
    cJSON *payload_dup = cJSON_Duplicate(payload, 1);
    cJSON_AddItemToObject(root, "payload_dup", payload_dup);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(30.0));
    cJSON_AddItemToObject(root, "arr2", arr);
    cJSON_DeleteItemFromArray(arr, 1);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *workbuf = (char *)cJSON_malloc(64);
    memset(workbuf, 0, 64);
    const char *name_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    cJSON *got_count_item = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "payload"), "count");
    double count_val = cJSON_GetNumberValue(got_count_item);
    cJSON *got_arr2 = cJSON_GetObjectItem(root, "arr2");
    int arr2_size = cJSON_GetArraySize(got_arr2);
    workbuf[0] = printed ? printed[0] : 'X';
    workbuf[1] = name_str ? name_str[0] : 'N';
    workbuf[2] = (char)('0' + ((int)count_val % 10));
    workbuf[3] = (char)('0' + (arr2_size % 10));
    (void)added_obj; (void)arr2_size; (void)workbuf;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(workbuf);
    cJSON_Delete(root);
    cJSON_free(json_buffer);
    return 66;
    // API sequence test completed successfully
}