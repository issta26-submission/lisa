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
//<ID> 439
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"meta\": {\"scale\": 2.5}, \"item\": {\"id\":\"abc\",\"value\":42}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *scale_item = cJSON_GetObjectItem(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    cJSON *item = cJSON_GetObjectItem(root, "item");
    cJSON *item_value = cJSON_GetObjectItem(item, "value");
    double value = cJSON_GetNumberValue(item_value);

    // step 2: Configure
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *item_ref = cJSON_CreateObjectReference(item);
    cJSON_AddItemToObject(config, "item_ref", item_ref);
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    (void)has_meta;

    // step 3: Operate
    double computed = scale * value;
    cJSON_AddNumberToObject(root, "computed_value", computed);
    cJSON *computed_item = cJSON_GetObjectItem(root, "computed_value");
    double retrieved = cJSON_GetNumberValue(computed_item);
    (void)retrieved;

    // step 4: Validate and Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}