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
//<ID> 1514
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *city = cJSON_CreateObject();
    cJSON_AddStringToObject(city, "name", "Metropolis");
    cJSON_AddNumberToObject(city, "population", 1000000.0);
    cJSON_AddItemToObject(root, "cityData", city);
    cJSON *info = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToObject(root, "mixed", arr);
    cJSON *temp = cJSON_CreateString("temporary");
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure
    const char json_text[] = "{\"alpha\":null,\"beta\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON_AddItemToObject(root, "parsed", parsed);
    char *printed = cJSON_PrintUnformatted(root);

    // step 3: Operate & Validate
    cJSON_bool has_info_before = cJSON_HasObjectItem(root, "info");
    cJSON *info_item = cJSON_GetObjectItem(root, "info");
    cJSON_bool is_info_null = cJSON_IsNull(info_item);
    cJSON *detached_temp = cJSON_DetachItemFromObject(root, "temp");
    cJSON_Delete(detached_temp);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "parsed");
    cJSON_bool has_parsed_after = cJSON_HasObjectItem(root, "parsed");
    (void)has_info_before;
    (void)is_info_null;
    (void)has_parsed_after;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}