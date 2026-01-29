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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[] = "  { \"name\" : \"example\" ,\n    \"value\" : 123 , \"flag\" : false }  ";
    size_t buf_len = sizeof(buf) - 1;
    cJSON *root = (cJSON *)0;
    cJSON *cs_item = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON_bool validated = 0;

    // step 2: Initialize - minify the JSON and parse with explicit length
    cJSON_Minify(buf);
    root = cJSON_ParseWithLength(buf, buf_len);

    // step 3: Configure - add a case-sensitive named string item and a boolean flag
    cs_item = cJSON_CreateString("added_case_sensitive");
    cJSON_AddItemToObjectCS(root, "case_name", cs_item);
    cJSON_AddBoolToObject(root, "parsed_ok", 1);

    // step 4: Operate/Validate - read back the boolean and store validation result
    flag_item = cJSON_GetObjectItem(root, "parsed_ok");
    validated = cJSON_IsTrue(flag_item);
    cJSON_AddBoolToObject(root, "validated", validated);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}