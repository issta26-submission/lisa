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
//<ID> 124
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
    cJSON *pi_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *twopi_item = NULL;
    cJSON_bool is_raw = 0;
    cJSON_bool equal = 0;
    double pi_value = 0.0;

    // step 2: Setup / Configure structure
    cJSON_AddNumberToObject(root, "pi", 3.141592653589793);
    cJSON_AddRawToObject(root, "note", "{\"raw\":true}");
    cJSON_AddNumberToObject(copy, "pi", 3.141592653589793);

    // step 3: Operate / Validate
    pi_item = cJSON_GetObjectItem(root, "pi");
    raw_item = cJSON_GetObjectItem(root, "note");
    pi_value = cJSON_GetNumberValue(pi_item);
    twopi_item = cJSON_AddNumberToObject(root, "twopi", pi_value * 2.0);
    is_raw = cJSON_IsRaw(raw_item);
    equal = cJSON_Compare(root, copy, 1);
    cJSON_AddBoolToObject(root, "note_is_raw", is_raw);
    cJSON_AddBoolToObject(root, "equal_copy", equal);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(copy);

    // API sequence test completed successfully
    return 66;
}