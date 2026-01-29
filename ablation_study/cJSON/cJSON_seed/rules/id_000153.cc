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
//<ID> 153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON *label_item = cJSON_CreateString("circle");
    char *printed = NULL;
    double pi_value = 0.0;
    cJSON_bool replaced = 0;
    cJSON *raw_meta = NULL;
    cJSON *found_pi = NULL;
    cJSON *replacement_pi = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "pi", pi_item);
    cJSON_AddItemToObject(root, "label", label_item);
    raw_meta = cJSON_AddRawToObject(root, "meta", "{\"unit\":\"radians\",\"approx\":true}");

    // step 3: Operate / Validate
    found_pi = cJSON_GetObjectItem(root, "pi");
    pi_value = cJSON_GetNumberValue(found_pi);
    replacement_pi = cJSON_CreateNumber(pi_value * 2.0);
    replaced = cJSON_ReplaceItemViaPointer(root, found_pi, replacement_pi);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)raw_meta;
    (void)replaced;
    return 66;
}