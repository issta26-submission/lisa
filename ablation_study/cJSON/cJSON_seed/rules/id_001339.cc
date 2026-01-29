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
//<ID> 1339
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"num\": 10.5, \"text\": \"hello\"}";
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *pi_item = NULL;
    cJSON *greet_item = NULL;
    cJSON *dup = NULL;
    char *printed = NULL;
    double num_val = 0.0;
    double pi_val = 0.0;
    const char *greet_str = NULL;
    cJSON_bool eq = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi_item);
    greet_item = cJSON_CreateString("world");
    cJSON_AddItemToObject(root, "greet", greet_item);

    // step 3: Operate / Validate
    num_item = cJSON_GetObjectItem(root, "num");
    num_val = cJSON_GetNumberValue(num_item);
    pi_item = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(pi_item);
    greet_item = cJSON_GetObjectItem(root, "greet");
    greet_str = cJSON_GetStringValue(greet_item);
    printed = cJSON_PrintUnformatted(root);
    dup = cJSON_Duplicate(root, 1);
    eq = cJSON_Compare(root, dup, 1);
    (void)greet_str;
    (void)num_val;
    (void)pi_val;
    (void)printed;
    (void)eq;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(dup);
    // API sequence test completed successfully
    return 66;
}