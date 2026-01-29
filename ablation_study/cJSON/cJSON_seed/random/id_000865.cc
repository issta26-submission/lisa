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
//<ID> 865
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json[] = "[10,20,30]";
    const char * parse_end = (const char *)0;
    size_t json_len = sizeof(json) - 1;
    cJSON * parsed = (cJSON *)0;
    cJSON * root = (cJSON *)0;
    cJSON * item = (cJSON *)0;
    cJSON * doubled = (cJSON *)0;
    double value = 0.0;
    cJSON_bool added_array = 0;
    cJSON_bool added_doubled = 0;
    int validation_score = 0;

    // step 2: Parse input with explicit length/options
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 3: Operate on parsed array (retrieve element and extract numeric value)
    item = cJSON_GetArrayItem(parsed, 1);
    value = cJSON_GetNumberValue(item);

    // step 4: Configure object and add items (create root, create new number, add parsed array and new number)
    root = cJSON_CreateObject();
    doubled = cJSON_CreateNumber(value * 2.0);
    added_array = cJSON_AddItemToObject(root, "numbers", parsed);
    added_doubled = cJSON_AddItemToObject(root, "doubled", doubled);

    // step 5: Validate results (aggregate simple checks without branches)
    validation_score = (int)(parsed != (cJSON *)0) + (int)(item != (cJSON *)0) + (int)added_array + (int)added_doubled + (int)(value == 20.0) + (int)(doubled != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}