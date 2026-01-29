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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"num\":1,\"str\":\"old\",\"obj\":{\"k\":10}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *obj_child = cJSON_GetObjectItem(root, "obj");
    cJSON *k_item = cJSON_GetObjectItem(obj_child, "k");

    // step 2: Configure
    cJSON *num_item = cJSON_CreateNumber(0.0);
    double setnum = cJSON_SetNumberHelper(num_item, 3.1415);
    cJSON_AddItemToObject(root, "pi", num_item);
    cJSON *str_item = cJSON_CreateString("placeholder");
    char *newstr = cJSON_SetValuestring(str_item, "updated");
    cJSON_AddItemToObject(root, "str2", str_item);
    cJSON *obj_ref = cJSON_CreateObjectReference(obj_child);
    cJSON_AddItemToObject(root, "objRef", obj_ref);
    cJSON_AddRawToObject(root, "rawData", "{\"rawKey\":123}");

    // step 3: Operate and Validate
    cJSON *got_pi = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(got_pi);
    double k_val = cJSON_GetNumberValue(k_item);
    cJSON *got_str = cJSON_GetObjectItem(root, "str2");
    const char *str_val = cJSON_GetStringValue(got_str);
    cJSON *got_raw = cJSON_GetObjectItem(root, "rawData");
    char *printed_raw = cJSON_PrintUnformatted(got_raw);

    // step 4: Cleanup
    (void)setnum;
    (void)pi_val;
    (void)k_val;
    (void)str_val;
    (void)newstr;
    cJSON_free(printed_raw);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}