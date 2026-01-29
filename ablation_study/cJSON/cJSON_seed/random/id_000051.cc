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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char json_buffer[] = " { \"user\": { \"id\": 42, \"active\": false }, \"tags\": [\"a\",\"b\"] } ";
    size_t json_len = (size_t)(sizeof(json_buffer) - 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = (cJSON *)0;
    cJSON *data = (cJSON *)0;
    cJSON *user = (cJSON *)0;
    double id_value = 0.0;

    // step 2: Setup - minify and parse input JSON
    cJSON_Minify(json_buffer);
    parsed = cJSON_ParseWithLength(json_buffer, json_len);

    // step 3: Configure - attach parsed JSON into root using case-sensitive add
    cJSON_AddItemToObjectCS(root, "data", parsed);
    cJSON_AddBoolToObject(root, "processed", (cJSON_bool)1);

    // step 4: Operate / Validate - extract nested values and add validation flag
    data = cJSON_GetObjectItem(root, "data");
    user = cJSON_GetObjectItem(data, "user");
    id_value = cJSON_GetNumberValue(cJSON_GetObjectItem(user, "id"));
    cJSON_AddBoolToObject(root, "id_is_42", (cJSON_bool)(id_value == 42.0));

    // step 5: Cleanup - free all allocated cJSON structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}