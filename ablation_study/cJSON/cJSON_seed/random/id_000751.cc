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
//<ID> 751
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *version_item = (cJSON *)0;
    char *printed = (char *)0;
    const char *version_str = (const char *)0;
    cJSON_bool added_greeting = 0;
    cJSON_bool added_version = 0;
    int validation_score = 0;
    char json_buf[] = "  {\n    \"sample\" : \"  spaced value \"\n  }\n";

    // step 2: Initialize
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString("Hello World");
    version_str = cJSON_Version();
    version_item = cJSON_CreateString(version_str);

    // step 3: Configure
    added_greeting = cJSON_AddItemToObject(root, "greeting", greeting);
    added_version = cJSON_AddItemToObject(root, "version", version_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_buf);

    // step 5: Validate
    validation_score = (printed != (char *)0) + (int)added_greeting + (int)added_version + (json_buf[0] == '{');
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}