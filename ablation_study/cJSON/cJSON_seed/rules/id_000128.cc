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
//<ID> 128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metadata = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *raw = cJSON_CreateRaw("raw_payload");
    cJSON *dup = NULL;
    cJSON *got_n = NULL;
    char *printed = NULL;
    double first_val = 0.0;
    double retrieved = 0.0;
    cJSON_bool equal = 0;
    cJSON_bool raw_flag = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToObject(root, "meta", metadata);
    cJSON_AddItemToObject(metadata, "note", cJSON_CreateString("example"));
    cJSON_AddItemToObject(metadata, "raw", raw);

    // step 3: Operate / Validate
    got_n = cJSON_GetArrayItem(values, 0);
    first_val = cJSON_GetNumberValue(got_n);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    dup = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup, 1);
    raw_flag = cJSON_IsRaw(cJSON_GetObjectItem(metadata, "raw"));
    cJSON_AddNumberToObject(metadata, "comparison_equal", equal ? 1.0 : 0.0);
    cJSON_AddNumberToObject(metadata, "is_raw", raw_flag ? 1.0 : 0.0);
    retrieved = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "first_value"));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}