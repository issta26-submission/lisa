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
//<ID> 225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *parsed = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *added_bool = (cJSON *)0;
    char *printed = (char *)0;
    const char *json_text = "{\"value\": 42, \"name\": \"example\"}";
    const char *parse_end = (const char *)0;
    size_t json_len = (sizeof("{\"value\": 42, \"name\": \"example\"}") - 1);
    double extracted_value = 0.0;
    cJSON_bool is_obj = (cJSON_bool)0;

    // step 2: Initialize (parse with explicit length and null-termination requirement)
    parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, (cJSON_bool)1);

    // step 3: Configure (inspect and annotate the parsed object)
    is_obj = cJSON_IsObject(parsed);
    added_bool = cJSON_AddBoolToObject(parsed, "is_object", is_obj);
    cJSON_AddStringToObject(parsed, "origin", "buffered");

    // step 4: Operate (extract numeric value and re-add it)
    num_item = cJSON_GetObjectItem(parsed, "value");
    extracted_value = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(parsed, "extracted_value", extracted_value);

    // step 5: Validate (serialize and free the serialization buffer)
    printed = cJSON_PrintUnformatted(parsed);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}