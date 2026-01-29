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
//<ID> 388
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"tester\",\"nums\":[1,2,3]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);
    const char *ver_str = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver_str);
    cJSON_AddItemToObject(root, "cjson_version", ver_item);

    // step 2: Configure
    cJSON *extra = cJSON_CreateArray();
    cJSON_AddItemToArray(extra, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(extra, cJSON_CreateNumber(7.5));
    cJSON_AddItemToObject(root, "extra", extra);
    cJSON *orig_nums = cJSON_GetObjectItem(root, "nums");
    cJSON *copied_nums = cJSON_Duplicate(orig_nums, 1);
    cJSON_AddItemToObject(root, "copied_nums", copied_nums);

    // step 3: Operate and Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(orig_nums, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(orig_nums, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(orig_nums, 2));
    double total = v0 + v1 + v2;
    cJSON_AddItemToObject(root, "sum", cJSON_CreateNumber(total));
    cJSON_bool has_name = cJSON_HasObjectItem(root, "name");
    cJSON_AddItemToObject(root, "has_name", cJSON_CreateBool(has_name));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}