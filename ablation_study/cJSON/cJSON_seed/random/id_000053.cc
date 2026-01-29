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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char json[] = "  /* sample comment */\n  { \"enabled\" : true, \"count\" : 123 }  ";
    size_t json_len = sizeof(json) - 1;
    cJSON *parsed = (cJSON *)0;
    cJSON *container = (cJSON *)0;

    // step 2: Setup - minify the JSON buffer to remove comments/whitespace
    cJSON_Minify(json);

    // step 3: Parse with length
    parsed = cJSON_ParseWithLength(json, json_len);

    // step 4: Configure/Integrate - create container and attach parsed object and a bool flag
    container = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(container, "parsed", parsed);
    cJSON_AddBoolToObject(container, "was_minified", 1);

    // step 5: Cleanup - free all allocated cJSON structures
    cJSON_Delete(container);

    // API sequence test completed successfully
    return 66;
}