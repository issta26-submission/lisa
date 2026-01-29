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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *owner_array = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *first_in_parsed = (cJSON *)0;
    char *unformatted = (char *)0;
    const char *parse_end = (const char *)0;
    cJSON_bool first_was_false = (cJSON_bool)0;
    const char json_buf[] = " [ false , true ] ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    owner_array = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(owner_array, false_item);
    cJSON_AddItemToObject(root, "owner", owner_array);

    // step 3: Configure (parse external JSON and attach)
    parsed = cJSON_ParseWithOpts(json_buf, &parse_end, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 4: Operate (inspect parsed content and produce an unformatted print)
    first_in_parsed = cJSON_GetArrayItem(parsed, 0);
    unformatted = cJSON_PrintUnformatted(parsed);
    cJSON_AddStringToObject(root, "parsed_unformatted", unformatted);
    cJSON_free(unformatted);

    // step 5: Validate (check that the first parsed element is false and record)
    first_was_false = cJSON_IsFalse(first_in_parsed);
    cJSON_AddBoolToObject(root, "first_was_false", first_was_false);

    // step 6: Cleanup
    cJSON_DeleteItemFromObject(root, "parsed");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}