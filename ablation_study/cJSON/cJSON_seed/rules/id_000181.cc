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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(123.456);
    cJSON *str_item = cJSON_CreateString("hello world");
    const char *version = cJSON_Version();
    char *tmpbuf = (char *)cJSON_malloc(32);
    memset(tmpbuf, 0, 32);
    tmpbuf[0] = version[0];
    tmpbuf[1] = '\0';

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(root, "greet", str_item);
    cJSON_AddStringToObject(root, "cvers", tmpbuf);

    // step 3: Operate / Validate
    cJSON *got_num = cJSON_GetObjectItem(root, "value");
    double extracted_number = cJSON_GetNumberValue(got_num);
    cJSON *got_greet = cJSON_GetObjectItem(root, "greet");
    cJSON_bool greet_is_string = cJSON_IsString(got_greet);
    (void)extracted_number;
    (void)greet_is_string;

    // step 4: Cleanup
    cJSON_free(tmpbuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}