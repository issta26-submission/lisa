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
//<ID> 1421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *dup = NULL;
    cJSON *subobj = NULL;
    char *printed = NULL;
    char json_str[] = "{\"alpha\":123, \"beta\":{\"inner\":true}}";
    char temp_buf[128];

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    parsed = cJSON_Parse(json_str);
    cJSON_AddItemToObject(root, "parsed", parsed);
    subobj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "sub", subobj);
    dup = cJSON_Duplicate(parsed, 1);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "sub", dup);

    // step 3: Operate / Validate
    memset(temp_buf, 0, sizeof(temp_buf));
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}