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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char mutable_json[] = " { \"alpha\" : { \"beta\" : false }, \"items\" : [ true , false ] } ";
    cJSON *parsed = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *dup = (cJSON *)0;

    // step 2: Setup - minify the JSON buffer to remove whitespace
    cJSON_Minify(mutable_json);

    // step 3: Operate - parse the minified JSON using its buffer length
    parsed = cJSON_ParseWithLength(mutable_json, (size_t)(sizeof(mutable_json) - 1));

    // step 4: Configure - create a new root object and import a duplicate of the parsed tree
    root = cJSON_CreateObject();
    dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObjectCS(root, "imported", dup);
    cJSON_AddBoolToObject(root, "processed", 1);

    // step 5: Validate - record whether the parsed top-level node is an object
    cJSON_AddBoolToObject(root, "is_object", cJSON_IsObject(parsed));

    // step 6: Cleanup - free allocated cJSON structures
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}