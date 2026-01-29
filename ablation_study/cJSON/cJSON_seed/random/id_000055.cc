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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char json[] = "  { \"name\" : \"test\", \"flag\": true, \"values\": [1, 2, 3] }  ";
    size_t len = sizeof(json) - 1;
    cJSON *parsed = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON_bool is_obj = 0;

    // step 2: Setup - minify input and parse with explicit length
    cJSON_Minify(json);
    parsed = cJSON_ParseWithLength(json, len);

    // step 3: Configure - create root object and attach parsed content (case-sensitive key)
    root = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "data", parsed);

    // step 4: Operate - inspect attached item and record a boolean property
    is_obj = cJSON_IsObject(parsed);
    cJSON_AddBoolToObject(root, "is_object", is_obj);

    // step 5: Validate/Integrate - add a duplicate of the object for demonstration
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_AddItemToObjectCS(root, "copy", dup);

    // step 6: Cleanup - free all allocated structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}