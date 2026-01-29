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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi = cJSON_CreateNumber(3.14159265);
    cJSON *replacement = cJSON_CreateString("pi_replaced");
    cJSON *raw_added = NULL;
    cJSON *retrieved_pi = NULL;
    cJSON *double_pi = NULL;
    char *printed = NULL;
    double pi_value = 0.0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "pi", pi);
    raw_added = cJSON_AddRawToObject(root, "raw", "{\"inner\":true}");
    cJSON_AddNumberToObject(root, "count", 7.0);

    // step 3: Operate / Validate
    retrieved_pi = cJSON_GetObjectItem(root, "pi");
    pi_value = cJSON_GetNumberValue(retrieved_pi);
    double_pi = cJSON_CreateNumber(pi_value * 2.0);
    cJSON_AddItemToObject(root, "double_pi", double_pi);
    replaced = cJSON_ReplaceItemViaPointer(root, retrieved_pi, replacement);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}