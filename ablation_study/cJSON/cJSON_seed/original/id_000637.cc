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
//<ID> 637
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"x\":10,\"y\":\"raw\"}";
    cJSON *root = cJSON_CreateObject();
    cJSON *a = cJSON_AddObjectToObject(root, "a");
    cJSON_AddNumberToObject(a, "id", 123.0);
    cJSON_AddTrueToObject(a, "valid");
    cJSON *b = cJSON_CreateObject();
    cJSON_AddNumberToObject(b, "id", 123.0);
    cJSON_AddTrueToObject(b, "valid");
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw", raw_json);

    // step 2: Configure
    cJSON_bool a_is_object = cJSON_IsObject(a);
    cJSON_AddBoolToObject(root, "a_is_object", a_is_object);
    cJSON_bool raw_is_object = cJSON_IsObject(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_object", raw_is_object);

    // step 3: Operate
    cJSON_bool are_equal = cJSON_Compare(a, b, 1);
    cJSON_AddBoolToObject(root, "a_equals_b", are_equal);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)printed;
    if (printed) { cJSON_free(printed); } /* harmless single-line cleanup without altering control flow */
    cJSON_Delete(b);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}