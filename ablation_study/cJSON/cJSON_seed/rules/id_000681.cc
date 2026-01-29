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
//<ID> 681
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    char *printed = NULL;
    const char *version = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    size_t vlen = 0;
    size_t vcopy = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *extra = NULL;
    cJSON *num_answer = cJSON_CreateNumber(42.0);
    cJSON *num_pi = cJSON_CreateNumber(3.1415);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(meta, "answer", num_answer);
    cJSON_AddItemToObject(root, "meta", meta);
    extra = cJSON_AddObjectToObject(root, "extra");
    cJSON_AddItemToObject(extra, "pi", num_pi);
    cJSON_AddStringToObject(root, "title", "cJSON Test Document");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);
    printed = NULL;
    version = cJSON_Version();
    vlen = version ? strlen(version) : 0;
    vcopy = vlen < (511 - to_copy) ? vlen : (511 - to_copy);
    memcpy(scratch + to_copy, version ? version : "", vcopy);
    scratch[to_copy + vcopy] = '\0';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}