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
//<ID> 228
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"num\": 42, \"text\": \"hello\"}";
    const char *parse_end = (const char *)0;
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *text_item = (cJSON *)0;
    cJSON_bool is_obj = (cJSON_bool)0;
    double num_value = 0.0;
    const char *text_value = (const char *)0;
    char numbuf[64];

    // step 2: Initialize (parse input with length and null-termination requirement)
    root = cJSON_ParseWithLengthOpts(json_text, strlen(json_text), &parse_end, (cJSON_bool)1);

    // step 3: Configure (determine type and record it into the parsed object)
    is_obj = cJSON_IsObject(root);
    cJSON_AddBoolToObject(root, "is_object", is_obj);

    // step 4: Operate (extract items and values, convert number to string)
    num_item = cJSON_GetObjectItem(root, "num");
    text_item = cJSON_GetObjectItem(root, "text");
    num_value = cJSON_GetNumberValue(num_item);
    text_value = cJSON_GetStringValue(text_item);
    snprintf(numbuf, sizeof(numbuf), "%.17g", num_value);
    cJSON_AddStringToObject(root, "num_as_string", numbuf);
    cJSON_AddStringToObject(root, "original_text", text_value);

    // step 5: Validate (no branching; just ensure data was added by using API that would be used in validation)
    cJSON_AddStringToObject(root, "parsed_tail", parse_end ? parse_end : "");

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}