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
//<ID> 1003
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *meta = NULL;
    cJSON *name = NULL;
    cJSON *values = NULL;
    cJSON *num1 = NULL;
    cJSON *null_field = NULL;
    cJSON *retrieved_meta = NULL;
    cJSON *retrieved_name = NULL;
    cJSON *first_elem = NULL;
    cJSON *dup = NULL;
    char *printed = NULL;
    const char *name_text = "deviceX";
    double first_val = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    name = cJSON_CreateString(name_text);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddTrueToObject(root, "active");
    null_field = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", null_field);
    values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddNumberToObject(root, "id", 123.0);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    retrieved_meta = cJSON_GetObjectItem(root, "meta");
    retrieved_name = cJSON_GetObjectItem(retrieved_meta, "name");
    first_elem = cJSON_GetArrayItem(values, 0);
    first_val = cJSON_GetNumberValue(first_elem);
    dup = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}