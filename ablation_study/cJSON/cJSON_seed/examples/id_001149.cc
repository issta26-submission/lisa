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
//<ID> 1149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *arr_text = "[10,20,30]";
    size_t arr_len = 10;
    cJSON *parsed_arr = cJSON_ParseWithLength(arr_text, arr_len);
    const char *obj_text = "{\"name\":\"alpha\",\"count\":2}";
    cJSON *parsed_obj = cJSON_Parse(obj_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(7);
    cJSON *n1 = cJSON_CreateNumber(8);
    cJSON *n2 = cJSON_CreateNumber(9);

    // step 2: Configure
    cJSON_AddItemToArray(created_arr, n0);
    cJSON_AddItemToArray(created_arr, n1);
    cJSON_AddItemToArray(created_arr, n2);
    cJSON_AddItemToObject(root, "created", created_arr);
    cJSON_AddItemToObject(root, "null_field", cJSON_CreateNull());
    cJSON *dup_obj = cJSON_Duplicate(parsed_obj, 1);
    cJSON_AddItemToObject(root, "parsed_obj", dup_obj);

    // step 3: Operate & Validate
    cJSON *item_from_parsed = cJSON_GetArrayItem(parsed_arr, 1);
    double parsed_val = cJSON_GetNumberValue(item_from_parsed);
    cJSON *computed = cJSON_CreateNumber(parsed_val + 5.5);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON *first_created = cJSON_GetArrayItem(created_arr, 0);
    double first_val = cJSON_GetNumberValue(first_created);
    cJSON_AddNumberToObject(root, "first_plus_one", first_val + 1.0);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed_arr);
    cJSON_Delete(parsed_obj);
    return 66; // API sequence test completed successfully
}