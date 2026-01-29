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
//<ID> 1275
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *manual_num = cJSON_CreateNumber(42.0);
    cJSON *null_item = cJSON_CreateNull();
    cJSON *greeting_item = NULL;
    cJSON *retrieved_manual = NULL;
    cJSON *retrieved_auto = NULL;
    char *greeting_str = NULL;
    double val_manual = 0.0;
    double val_auto = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "manual_number", manual_num);
    cJSON_AddNumberToObject(root, "auto_number", 3.14159);
    cJSON_AddStringToObject(root, "greeting", "hello world");
    cJSON_AddItemToObject(root, "nothing", null_item);

    // step 3: Operate / Validate
    greeting_item = cJSON_GetObjectItem(root, "greeting");
    greeting_str = cJSON_GetStringValue(greeting_item);
    retrieved_manual = cJSON_GetObjectItem(root, "manual_number");
    val_manual = cJSON_GetNumberValue(retrieved_manual);
    retrieved_auto = cJSON_GetObjectItem(root, "auto_number");
    val_auto = cJSON_GetNumberValue(retrieved_auto);
    printed = cJSON_PrintUnformatted(root);
    (void)greeting_str;
    (void)val_manual;
    (void)val_auto;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}