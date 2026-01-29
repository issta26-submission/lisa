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
//<ID> 388
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *num_item = NULL;
    cJSON *sref = NULL;
    cJSON *s_item = NULL;
    double numval = 0.0;
    cJSON_bool raw_flag = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    sref = cJSON_CreateStringReference("hello");
    cJSON_AddItemToObject(root, "greeting", sref);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    num_item = cJSON_GetObjectItem(parsed, "count");
    numval = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(parsed, "double_count", numval * 2.0);
    s_item = cJSON_GetObjectItem(parsed, "greeting");
    raw_flag = cJSON_IsRaw(s_item);
    buffer[0] = (char)('0' + (int)numval);
    buffer[1] = (char)('0' + (int)raw_flag);
    buffer[2] = '\0';
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "greeting");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}