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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON *item_initial = NULL;
    cJSON *item_label = NULL;
    cJSON *item_nested = NULL;
    cJSON *item_n = NULL;
    char *str_label = NULL;
    char *printed = NULL;
    double num_initial = 0.0;
    double num_n = 0.0;
    double total = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "initial", 3.14);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("example"));
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddNumberToObject(nested, "n", 7.0);

    // step 3: Operate / Validate
    item_initial = cJSON_GetObjectItem(root, "initial");
    num_initial = cJSON_GetNumberValue(item_initial);
    item_label = cJSON_GetObjectItem(root, "label");
    str_label = cJSON_GetStringValue(item_label);
    item_nested = cJSON_GetObjectItem(root, "nested");
    item_n = cJSON_GetObjectItem(item_nested, "n");
    num_n = cJSON_GetNumberValue(item_n);
    total = num_initial + num_n + (double)(unsigned char)str_label[0];
    cJSON_AddNumberToObject(root, "total", total);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}