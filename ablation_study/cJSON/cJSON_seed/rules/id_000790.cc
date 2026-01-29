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
//<ID> 790
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *raw_arr = NULL;
    cJSON *parsed_arr = NULL;
    const char *version = NULL;
    int raw_size = 0;
    int parsed_size = 0;
    int total_size = 0;
    char *printed = NULL;
    char *copied = NULL;
    size_t printed_len = 0;

    // step 2: Setup / Configure
    version = cJSON_Version();
    root = cJSON_CreateObject();
    raw_arr = cJSON_CreateRaw("[10,20,30]");
    parsed_arr = cJSON_Parse("[1,2,3,4]");
    cJSON_AddItemToObject(root, "raw_array", raw_arr);
    cJSON_AddItemToObject(root, "parsed_array", parsed_arr);

    // step 3: Operate / Validate
    raw_size = cJSON_GetArraySize(raw_arr);
    parsed_size = cJSON_GetArraySize(parsed_arr);
    total_size = raw_size + parsed_size;
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    copied = (char *)cJSON_malloc(printed_len + 1);
    memset(copied, 0, printed_len + 1);
    memcpy(copied, printed, printed_len + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copied);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)version;
    (void)raw_size;
    (void)parsed_size;
    (void)total_size;
    return 66;
}