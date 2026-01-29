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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char json_buf[] = " { \"alpha\" : 123, \"beta\" : \"text with spaces\" , \"flag\": false, \"nested\": { \"x\": true } } ";
    cJSON *parsed = (cJSON *)0;
    cJSON *root = cJSON_CreateObject();
    cJSON *note = cJSON_CreateString("constructed");
    cJSON *embedded = (cJSON *)0;
    cJSON *nested = (cJSON *)0;
    cJSON *xitem = (cJSON *)0;
    cJSON_bool nested_x_is_true = 0;
    size_t len = 0;

    // step 2: Setup - minify and parse the JSON buffer
    cJSON_Minify(json_buf);
    len = strlen(json_buf);
    parsed = cJSON_ParseWithLength(json_buf, len);

    // step 3: Configure - attach parsed object and an auxiliary string using CS insertion
    cJSON_AddItemToObjectCS(root, "embedded", parsed);
    cJSON_AddItemToObjectCS(root, "note", note);

    // step 4: Operate - add a boolean flag into root using cJSON_AddBoolToObject
    cJSON_AddBoolToObject(root, "status", 1);

    // step 5: Validate - inspect nested.x from the embedded object and record its truth value
    embedded = cJSON_GetObjectItem(root, "embedded");
    nested = cJSON_GetObjectItem(embedded, "nested");
    xitem = cJSON_GetObjectItem(nested, "x");
    nested_x_is_true = cJSON_IsTrue(xitem);
    cJSON_AddBoolToObject(root, "nested_x_true", nested_x_is_true);

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}