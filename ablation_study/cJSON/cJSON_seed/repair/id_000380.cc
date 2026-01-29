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
//<ID> 380
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddStringToObject(info, "name", "example");
    const char *ver = cJSON_Version();
    cJSON_AddStringToObject(info, "version", ver);

    // step 3: Operate and Validate
    const char *json_text = "{\"present\":true,\"value\":42}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON_bool has_present = cJSON_HasObjectItem(parsed, "present");
    cJSON *present_bool = cJSON_CreateBool(has_present);
    cJSON_AddItemToObject(root, "parsed_has_present", present_bool);
    cJSON *value_item = cJSON_GetObjectItem(parsed, "value");
    double combined = cJSON_GetNumberValue(value_item) + cJSON_GetNumberValue(n1);
    cJSON *computed = cJSON_CreateNumber(combined);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 4: Cleanup
    int arr_size = cJSON_GetArraySize(arr);
    (void)arr_size;
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}