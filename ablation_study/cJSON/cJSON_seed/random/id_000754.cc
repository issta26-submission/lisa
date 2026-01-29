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
//<ID> 754
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *ver_item = (cJSON *)0;
    const char *version_str = (const char *)0;
    char *printed = (char *)0;
    cJSON_bool added_injected = 0;
    cJSON_bool added_version = 0;
    int validation_score = 0;

    // step 2: Initialize
    char json_buf[] = "  { \"name\": \"Alice\", \"city\": \"Wonderland\" }  ";
    cJSON_Minify(json_buf);
    parsed = cJSON_Parse(json_buf);
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("injected_name");
    version_str = cJSON_Version();
    ver_item = cJSON_CreateString(version_str);

    // step 3: Configure
    added_injected = cJSON_AddItemToObject(parsed, "injected", name_item);
    added_version = cJSON_AddItemToObject(parsed, "version", ver_item);
    cJSON_AddItemToObject(root, "wrapped", parsed);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (printed != (char *)0) + (int)added_injected + (int)added_version;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}