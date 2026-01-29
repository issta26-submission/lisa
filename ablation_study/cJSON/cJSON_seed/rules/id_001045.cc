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
//<ID> 1045
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *items = NULL;
    cJSON *first_item = NULL;
    cJSON *second_item = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "device", "sensor-01");
    items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateString("temperature"));
    cJSON_AddItemToArray(items, cJSON_CreateString("humidity"));
    cJSON_AddStringToObject(root, "status", "active");

    // step 3: Operate / Validate
    first_item = cJSON_GetArrayItem(items, 0);
    second_item = cJSON_GetArrayItem(items, 1);
    (void)first_item;
    (void)second_item;
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}