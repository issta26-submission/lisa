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
//<ID> 97
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
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON *raw = cJSON_CreateRaw("{\"x\":true}");
    cJSON *tmp = NULL;
    char *printed = NULL;
    double extracted_value = 0.0;
    cJSON_bool compare_result = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(nested, "n", num1);
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddItemToObject(root, "dup", num2);
    cJSON_AddItemToObject(root, "raw_payload", raw);
    cJSON_AddNumberToObject(root, "direct", 3.14);

    // step 3: Operate / Validate
    tmp = cJSON_GetObjectItem(root, "nested");
    tmp = cJSON_GetObjectItem(tmp, "n");
    extracted_value = cJSON_GetNumberValue(tmp);
    compare_result = cJSON_Compare(tmp, cJSON_GetObjectItem(root, "dup"), 1);
    cJSON_AddNumberToObject(root, "cmp", (double)compare_result);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}