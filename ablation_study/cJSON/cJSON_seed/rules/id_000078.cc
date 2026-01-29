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
//<ID> 78
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
    cJSON *label = cJSON_CreateString("example_label");
    cJSON *num_item = NULL;
    cJSON *found_child = NULL;
    char *str_val = NULL;
    char *printed = NULL;
    double value = 0.0;
    double base = 0.0;
    double sum = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "label", label);
    cJSON_AddNumberToObject(child, "value", 5.0);
    cJSON_AddNumberToObject(root, "base", 2.0);

    // step 3: Operate / Validate
    found_child = cJSON_GetObjectItem(root, "child");
    num_item = cJSON_GetObjectItem(found_child, "value");
    value = cJSON_GetNumberValue(num_item);
    str_val = cJSON_GetStringValue(cJSON_GetObjectItem(found_child, "label"));
    base = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "base"));
    sum = value + base;
    cJSON_AddNumberToObject(root, "sum", sum);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}