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
//<ID> 1174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"id\":42,\"active\":false}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *raw_payload = cJSON_CreateRaw("{\"inner\":7}");
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "raw_payload", raw_payload);
    cJSON_AddItemToObjectCS(root, "explicit_true", true_item);
    cJSON_AddBoolToObject(root, "flag_bool_via_api", 1);

    // step 3: Operate & Validate
    cJSON *id_item = cJSON_GetObjectItem(parsed, "id");
    double id_value = cJSON_GetNumberValue(id_item);
    cJSON *id_copy = cJSON_CreateNumber(id_value);
    cJSON_AddItemToObjectCS(root, "id_copy", id_copy);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}