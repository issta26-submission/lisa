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
//<ID> 1277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_obj = cJSON_CreateNumber(3.14159265);
    cJSON *null_obj = cJSON_CreateNull();
    cJSON *pi_item = NULL;
    cJSON *e_item = NULL;
    cJSON *greet_item = NULL;
    double pi_val = 0.0;
    double e_val = 0.0;
    char *greet_str = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "pi", num_obj);
    cJSON_AddItemToObject(root, "nullable", null_obj);
    cJSON_AddNumberToObject(root, "e", 2.71828182);
    cJSON_AddStringToObject(root, "greeting", "hello world");

    // step 3: Operate / Validate
    pi_item = cJSON_GetObjectItem(root, "pi");
    e_item = cJSON_GetObjectItem(root, "e");
    greet_item = cJSON_GetObjectItem(root, "greeting");
    pi_val = cJSON_GetNumberValue(pi_item);
    e_val = cJSON_GetNumberValue(e_item);
    greet_str = cJSON_GetStringValue(greet_item);
    printed = cJSON_PrintUnformatted(root);
    (void)pi_val;
    (void)e_val;
    (void)greet_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}