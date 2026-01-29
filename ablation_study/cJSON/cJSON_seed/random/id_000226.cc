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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json_text_literal[] = "{\"initial\": 10, \"name\": \"test\"}";
    size_t json_length = sizeof(json_text_literal) - 1;
    const char * parse_end = (const char *)0;
    cJSON * root = (cJSON *)0;
    cJSON * initial_item = (cJSON *)0;
    cJSON * name_item = (cJSON *)0;
    const char * name_value = (const char *)0;
    double initial_value = 0.0;
    char * printed = (char *)0;
    cJSON_bool is_obj = (cJSON_bool)0;

    // step 2: Initialize (parse JSON with explicit length and options)
    root = cJSON_ParseWithLengthOpts(json_text_literal, json_length, &parse_end, (cJSON_bool)1);

    // step 3: Configure (inspect parsed root and add fields)
    is_obj = cJSON_IsObject((const cJSON *const)root);
    cJSON_AddBoolToObject(root, "was_object", is_obj);
    name_item = cJSON_GetObjectItem(root, "name");
    name_value = (const char *)0;
    name_value = cJSON_GetStringValue((const cJSON *const)name_item);
    cJSON_AddStringToObject(root, "copied_name", name_value);

    // step 4: Operate (extract numeric value and add derived value)
    initial_item = cJSON_GetObjectItem(root, "initial");
    initial_value = cJSON_GetNumberValue((const cJSON *const)initial_item);
    cJSON_AddNumberToObject(root, "double_initial", initial_value * 2.0);

    // step 5: Validate (render and free printed JSON)
    printed = cJSON_PrintUnformatted((const cJSON * )root);
    cJSON_free((void *)printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}