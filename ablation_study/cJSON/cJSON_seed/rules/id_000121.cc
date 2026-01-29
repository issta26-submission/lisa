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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *dup = NULL;
    cJSON *pi_item = NULL;
    char *printed = NULL;
    double pi_value = 0.0;
    cJSON_bool is_raw = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "pi", 3.141592653589793);
    cJSON_AddItemToObject(root, "rawdata", raw);

    // step 3: Operate / Validate
    pi_item = cJSON_GetObjectItem(root, "pi");
    pi_value = cJSON_GetNumberValue(pi_item);
    cJSON_AddNumberToObject(root, "pi_times_two", pi_value * 2.0);
    is_raw = cJSON_IsRaw(raw);
    cJSON_AddBoolToObject(root, "raw_is_raw", is_raw);
    dup = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup, 1);
    cJSON_AddBoolToObject(root, "equal_dup", equal);
    printed = cJSON_PrintUnformatted(dup);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}