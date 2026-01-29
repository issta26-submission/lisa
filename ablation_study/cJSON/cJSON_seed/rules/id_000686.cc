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
//<ID> 686
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printed = NULL;
    const char *version = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = NULL;
    cJSON *pi_num = NULL;

    // step 2: Setup / Configure
    meta = cJSON_AddObjectToObject(root, "metadata");
    pi_num = cJSON_CreateNumber(3.141592653589793);
    cJSON_AddItemToObject(meta, "pi", pi_num);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);
    printed = NULL;
    version = cJSON_Version();
    memcpy(scratch + 128, version ? version : "", strlen(version ? version : ""));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}