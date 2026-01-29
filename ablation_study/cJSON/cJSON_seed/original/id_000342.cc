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
//<ID> 342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"config\":{\"threshold\":2.5},\"items\":[\"one\",\"two\"]}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *config = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *threshold = cJSON_GetObjectItemCaseSensitive(config, "threshold");
    double thr_value = cJSON_GetNumberValue(threshold);
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");

    // step 2: Configure
    cJSON *holder = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "holder", holder);
    cJSON *ref_item = cJSON_CreateString("referenced");
    cJSON_AddItemToObject(holder, "ref", ref_item);
    cJSON_bool addref_ok = cJSON_AddItemReferenceToArray(items, ref_item);
    cJSON *status_num = cJSON_CreateNumber((double)addref_ok);
    cJSON_AddItemToObject(holder, "addref_ok", status_num);

    // step 3: Operate
    cJSON *thr_node = cJSON_CreateNumber(thr_value);
    cJSON_AddItemToObject(holder, "thr_used", thr_node);
    char *printed = cJSON_Print(root);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}