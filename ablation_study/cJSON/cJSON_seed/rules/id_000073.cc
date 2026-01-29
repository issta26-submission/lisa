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
//<ID> 73
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *item_x = NULL;
    cJSON *item_y = NULL;
    cJSON *item_label = NULL;
    double x_value = 0.0;
    double y_value = 0.0;
    double total = 0.0;
    char *label_str = NULL;
    char *printed = NULL;

    // step 2: Setup
    item_x = cJSON_AddNumberToObject(root, "x", 3.14159);
    cJSON_AddNumberToObject(root, "y", 2.71828);
    cJSON_AddStringToObject(root, "label", "constants");

    // step 3: Operate / Validate
    item_x = cJSON_GetObjectItem(root, "x");
    x_value = cJSON_GetNumberValue(item_x);
    item_y = cJSON_GetObjectItem(root, "y");
    y_value = cJSON_GetNumberValue(item_y);
    item_label = cJSON_GetObjectItem(root, "label");
    label_str = cJSON_GetStringValue(item_label);
    total = x_value + y_value;
    cJSON_AddNumberToObject(root, "total", total);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}