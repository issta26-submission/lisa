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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *copy = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    const char *raw_text = "{\"enabled\":true}";
    cJSON *raw = cJSON_CreateRaw(raw_text);
    cJSON *num_copy = cJSON_CreateNumber(42.0);
    cJSON *raw_copy = cJSON_CreateRaw(raw_text);
    char *printed = NULL;
    cJSON *retrieved = NULL;
    double value = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddItemToObject(root, "meta", raw);
    cJSON_AddItemToObject(copy, "value", num_copy);
    cJSON_AddItemToObject(copy, "meta", raw_copy);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "value");
    value = cJSON_GetNumberValue(retrieved);
    printed = cJSON_PrintUnformatted(root);
    equal = cJSON_Compare(root, copy, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(copy);

    return 66;
    // API sequence test completed successfully
}