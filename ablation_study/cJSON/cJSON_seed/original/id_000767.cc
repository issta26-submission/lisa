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
//<ID> 767
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_text[] = " { \"alpha\" : true , \"beta\" : false } ";
    cJSON_Minify(json_text);
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();

    // step 2: Configure
    cJSON *added_bool = cJSON_AddBoolToObject(root, "active", 1);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "always_on", true_item);
    cJSON_AddBoolToObject(parsed, "newflag", 0);
    cJSON *parsed_ref = cJSON_CreateObjectReference(parsed);
    cJSON_AddItemToObjectCS(root, "parsedRef", parsed_ref);

    // step 3: Operate & Validate
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObjectCS(root, "childRef", child_ref);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    cJSON_Delete(child);
    // API sequence test completed successfully
    return 66;
}