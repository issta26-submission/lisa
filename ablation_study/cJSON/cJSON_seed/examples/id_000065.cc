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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToObjectCS(meta, "fixed", n1);
    cJSON_AddItemToObject(meta, "dynamic", n2);

    // step 3: Operate & Validate
    cJSON_bool has_fixed = cJSON_HasObjectItem(meta, "fixed");
    cJSON_bool has_dynamic = cJSON_HasObjectItem(meta, "dynamic");
    cJSON *dyn_item = cJSON_GetObjectItem(meta, "dynamic");
    double dyn_val = cJSON_GetNumberValue(dyn_item);
    double total_val = dyn_val + (double)has_fixed + (double)has_dynamic;
    cJSON *total_num = cJSON_CreateNumber(total_val);
    cJSON_AddItemToObject(root, "total", total_num);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}