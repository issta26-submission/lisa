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
//<ID> 1600
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *s3 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *raw = cJSON_CreateRaw("{\"rawkey\":123}");
    cJSON_AddItemToObject(root, "raw_payload", raw);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString("v1.0"));
    cJSON_AddItemToObject(root, "metadata", meta);

    // step 2: Configure
    int items_count = cJSON_GetArraySize(arr);
    cJSON *items_count_num = cJSON_CreateNumber((double)items_count);
    cJSON_AddItemToObject(root, "items_count", items_count_num);
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_AddItemToObject(root, "has_items", cJSON_CreateBool(has_items));
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_AddItemToObject(root, "metadata_is_object", cJSON_CreateBool(meta_is_obj));
    cJSON_bool has_raw = cJSON_HasObjectItem(root, "raw_payload");
    cJSON_AddItemToObject(root, "has_raw_payload", cJSON_CreateBool(has_raw));

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}