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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    char *printed = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON *num_custom = cJSON_CreateNumber(42.0);
    cJSON *parsed = NULL;
    cJSON *parsed_data = NULL;
    cJSON *parsed_answer = NULL;
    double answer_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON_AddItemToObject(nested, "answer", num_custom);
    cJSON_AddItemToObject(root, "data", nested);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    parsed_data = cJSON_GetObjectItem(parsed, "data");
    parsed_answer = cJSON_GetObjectItemCaseSensitive(parsed_data, "answer");
    answer_value = cJSON_GetNumberValue(parsed_answer);
    buffer[0] = (char)('0' + (int)answer_value); /* place first char based on number */
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "count");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}