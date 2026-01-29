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
//<ID> 761
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *num_item = NULL;
    cJSON *flag = NULL;
    cJSON *parsed_num_item = NULL;
    const char *parse_end = NULL;
    double initial_val = 0.0;
    double extracted_val = 0.0;
    char json_buf[] = "{\"parsed_num\": 123.456}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "initial", 3.14);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);
    parsed = cJSON_ParseWithLengthOpts(json_buf, (size_t)(sizeof(json_buf) - 1), &parse_end, 1);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate / Validate
    parsed_num_item = cJSON_GetObjectItem(parsed, "parsed_num");
    extracted_val = cJSON_GetNumberValue(parsed_num_item);
    initial_val = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "extracted", extracted_val + initial_val);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}