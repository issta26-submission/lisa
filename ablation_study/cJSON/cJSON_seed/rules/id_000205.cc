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
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(2.5);
    cJSON *str = cJSON_CreateString("label_text");
    cJSON *num2 = cJSON_CreateNumber(0.0);
    void *buffer = cJSON_malloc(64);
    memset(buffer, 0, 64);
    double set_ret = 0.0;
    double value = 0.0;
    const char *label = NULL;
    cJSON_bool flag_is_false = 0;
    char *json_unfmt = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(child, "num", num);
    cJSON_AddItemToObject(child, "label", str);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "computed", num2);
    cJSON_AddFalseToObject(root, "active");
    set_ret = cJSON_SetNumberHelper(num2, 42.42);

    // step 3: Operate / Validate
    cJSON *child_node = cJSON_GetObjectItem(root, "child");
    cJSON *num_node = cJSON_GetObjectItem(child_node, "num");
    value = cJSON_GetNumberValue(num_node);
    label = cJSON_GetStringValue(cJSON_GetObjectItem(child_node, "label"));
    flag_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "active"));
    json_unfmt = cJSON_PrintUnformatted(root);
    ((char *)buffer)[0] = json_unfmt[0];
    cJSON_free(json_unfmt);
    (void)set_ret;
    (void)value;
    (void)label;
    (void)flag_is_false;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}