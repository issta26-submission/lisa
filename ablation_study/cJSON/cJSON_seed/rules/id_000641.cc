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
//<ID> 641
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(1024);
    memset(scratch, 0, 1024);
    cJSON *root = cJSON_CreateObject();
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *s2 = cJSON_CreateString("world");

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "greeting", s1);
    cJSON_AddItemToObject(root, "target", s2);
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON_AddRawToObject(root, "raw_payload", "{\"nested\":true,\"count\":3}");

    // step 3: Operate / Validate
    char *printed_unf = cJSON_PrintUnformatted(root);
    size_t len_unf = printed_unf ? strlen(printed_unf) : 0;
    size_t copy_unf = len_unf < 511 ? len_unf : 511;
    memcpy(scratch, printed_unf ? printed_unf : "", copy_unf);
    scratch[copy_unf] = '\0';
    cJSON_free(printed_unf);
    char *printed = cJSON_Print(root);
    size_t len = printed ? strlen(printed) : 0;
    size_t copy_pre = len < 511 ? len : 511;
    memcpy(scratch + copy_unf + 1, printed ? printed : "", copy_pre);
    scratch[copy_unf + 1 + copy_pre] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}