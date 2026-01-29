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
//<ID> 1158
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
    cJSON *nested_obj = NULL;
    cJSON *nested_k = NULL;
    cJSON *num_item = NULL;
    const char *parse_end = NULL;
    double extracted_number = 0.0;
    const char *extracted_k = NULL;
    char mutable_json[] = "{  \"a\" :  42 ,  \"b\" : null ,  \"nested\" : { \"k\" : \"value\" } }";

    // step 2: Setup / Configure
    cJSON_Minify(mutable_json);
    parsed = cJSON_ParseWithLengthOpts(mutable_json, (size_t)(sizeof(mutable_json) - 1), &parse_end, 1);
    root = cJSON_CreateObject();

    // step 3: Operate / Validate
    cJSON_AddItemToObject(root, "parsed_payload", parsed);
    cJSON_AddNullToObject(root, "explicit_null");
    cJSON_AddItemToObject(root, "greeting", cJSON_CreateString("hello"));
    nested_obj = cJSON_GetObjectItem(parsed, "nested");
    nested_k = cJSON_GetObjectItem(nested_obj, "k");
    extracted_k = cJSON_GetStringValue(nested_k);
    num_item = cJSON_GetObjectItem(parsed, "a");
    extracted_number = cJSON_GetNumberValue(num_item);
    (void)extracted_k;
    (void)extracted_number;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}