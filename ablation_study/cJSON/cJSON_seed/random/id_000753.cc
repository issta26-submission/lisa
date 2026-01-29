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
//<ID> 753
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
    cJSON *target = (cJSON *)0;
    cJSON *compact = (cJSON *)0;
    cJSON_bool added_greeting = 0;
    cJSON_bool added_target = 0;
    cJSON_bool added_compact = 0;
    const char *version = (const char *)0;
    int validation_score = 0;
    char minify_buf[] = " { \"a\" : 1 , \"b\" : \"two\" } ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString("Hello");
    target = cJSON_CreateString("World");
    version = cJSON_Version();
    cJSON_Minify(minify_buf);
    compact = cJSON_CreateString(minify_buf);

    // step 3: Configure
    added_greeting = cJSON_AddItemToObject(root, "greeting", greeting);
    added_target = cJSON_AddItemToObject(root, "target", target);
    added_compact = cJSON_AddItemToObject(root, "compact", compact);

    // step 4: Operate
    (void)version;
    (void)minify_buf;

    // step 5: Validate
    validation_score = (root != (cJSON *)0) + (greeting != (cJSON *)0) + (target != (cJSON *)0) + (compact != (cJSON *)0) + (added_greeting != 0) + (added_target != 0) + (added_compact != 0) + (version != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}